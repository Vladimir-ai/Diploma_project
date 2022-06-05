#include "fast_feature_detector.h"
#include "opencv2/imgproc.hpp"


OpencvFastFeatureDetector::OpencvFastFeatureDetector(Statistics *stat,
                                                     int threshold,
                                                     bool nonmaxSuppression,
                                                     FastFeatureDetector::DetectorType type): IAbstractFeatureDetector(stat)
{
  m_detector = FastFeatureDetector::create(threshold, nonmaxSuppression, type);
  m_threshold = threshold;
  m_nonmax_suppression = nonmaxSuppression;

  switch(type)
  {
    case FastFeatureDetector::TYPE_5_8:
      m_pattern_size = 8;
      break;
    case FastFeatureDetector::TYPE_7_12:
      m_pattern_size = 12;
      break;
    case FastFeatureDetector::TYPE_9_16:
      m_pattern_size = 16;
      break;
  }
}


/* Need it to make row offsets for each pixel */
void OpencvFastFeatureDetector::makeOffsets(int pixel[], int row_stride)
{
    pixel[0] = 0 + row_stride * 3;
    pixel[1] = 1 + row_stride * 3;
    pixel[2] = 2 + row_stride * 2;
    pixel[3] = 3 + row_stride * 1;
    pixel[4] = 3 + row_stride * 0;
    pixel[5] = 3 + row_stride * -1;
    pixel[6] = 2 + row_stride * -2;
    pixel[7] = 1 + row_stride * -3;
    pixel[8] = 0 + row_stride * -3;
    pixel[9] = -1 + row_stride * -3;
    pixel[10] = -2 + row_stride * -2;
    pixel[11] = -3 + row_stride * -1;
    pixel[12] = -3 + row_stride * 0;
    pixel[13] = -3 + row_stride * 1;
    pixel[14] = -2 + row_stride * 2;
    pixel[15] = -1 + row_stride * 3;
}


int OpencvFastFeatureDetector::cornerScore(const uchar* ptr, const int pixel[], int threshold)
{
  const int K = 8, N = 16 + K + 1;
  int k, v = ptr[0];
  short d[N];
  for( k = 0; k < N; k++ )
      d[k] = (short)(v - ptr[pixel[k]]);

  int a0 = threshold;
  for( k = 0; k < 16; k += 2 )
  {
      int a = std::min((int)d[k+1], (int)d[k+2]);
      a = std::min(a, (int)d[k+3]);
      if( a <= a0 )
          continue;
      a = std::min(a, (int)d[k+4]);
      a = std::min(a, (int)d[k+5]);
      a = std::min(a, (int)d[k+6]);
      a = std::min(a, (int)d[k+7]);
      a = std::min(a, (int)d[k+8]);
      a0 = std::max(a0, std::min(a, (int)d[k]));
      a0 = std::max(a0, std::min(a, (int)d[k+9]));
  }

  int b0 = -a0;
  for( k = 0; k < 16; k += 2 )
  {
      int b = std::max((int)d[k+1], (int)d[k+2]);
      b = std::max(b, (int)d[k+3]);
      b = std::max(b, (int)d[k+4]);
      b = std::max(b, (int)d[k+5]);
      if( b >= b0 )
          continue;
      b = std::max(b, (int)d[k+6]);
      b = std::max(b, (int)d[k+7]);
      b = std::max(b, (int)d[k+8]);

      b0 = std::min(b0, std::max(b, (int)d[k]));
      b0 = std::min(b0, std::max(b, (int)d[k+9]));
  }

  threshold = -b0-1;

  return threshold;
}


bool OpencvFastFeatureDetector::detect_features(Mat frame, std::vector<Point2f> &points)
{
  const clock_t begin_time = clock();
  const int K = 8, N = 16 + K + 1;
  int i, j, k, pixel[N], threshold;
  Mat grayFrame;

  if( frame.type() != CV_8U )
  {
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
    frame = grayFrame;
  }

  makeOffsets(pixel, (int)frame.step);
  for(k = 16; k < N; k++)
      pixel[k] = pixel[k - 16];

  points.clear();

  threshold = std::min(std::max(m_threshold, 0), 255);

  uchar threshold_tab[512];
  for( i = -255; i <= 255; i++ )
      threshold_tab[i+255] = (uchar)(i < -threshold ? 1 : i > threshold ? 2 : 0);

  AutoBuffer<uchar> _buf((frame.cols+16)*3*(sizeof(int) + sizeof(uchar)) + 128);
  uchar* buf[3];
  buf[0] = _buf; buf[1] = buf[0] + frame.cols; buf[2] = buf[1] + frame.cols;
  int* cpbuf[3];
  cpbuf[0] = (int*)alignPtr(buf[2] + frame.cols, sizeof(int)) + 1;
  cpbuf[1] = cpbuf[0] + frame.cols + 1;
  cpbuf[2] = cpbuf[1] + frame.cols + 1;
  memset(buf[0], 0, frame.cols*3);

  for(i = 3; i < frame.rows-2; i++)
  {
    const uchar* ptr = frame.ptr<uchar>(i) + 3;
    uchar* curr = buf[(i - 3)%3];
    int* cornerpos = cpbuf[(i - 3)%3];
    memset(curr, 0, frame.cols);
    int ncorners = 0;

    if( i < frame.rows - 3 )
    {
      j = 3;
      for( ; j < frame.cols - 3; j++, ptr++ )
      {
        int v = ptr[0];
        const uchar* tab = &threshold_tab[0] - v + 255;
        int d = tab[ptr[pixel[0]]] | tab[ptr[pixel[8]]];

        if( d == 0 )
          continue;

        d &= tab[ptr[pixel[2]]] | tab[ptr[pixel[10]]];
        d &= tab[ptr[pixel[4]]] | tab[ptr[pixel[12]]];
        d &= tab[ptr[pixel[6]]] | tab[ptr[pixel[14]]];

        if( d == 0 )
          continue;

        d &= tab[ptr[pixel[1]]] | tab[ptr[pixel[9]]];
        d &= tab[ptr[pixel[3]]] | tab[ptr[pixel[11]]];
        d &= tab[ptr[pixel[5]]] | tab[ptr[pixel[13]]];
        d &= tab[ptr[pixel[7]]] | tab[ptr[pixel[15]]];

        if( d & 1 )
        {
          int vt = v - threshold, count = 0;

          for( k = 0; k < N; k++ )
          {
            int x = ptr[pixel[k]];
            if(x < vt)
            {
              if( ++count > K )
              {
                cornerpos[ncorners++] = j;
                if(m_nonmax_suppression)
                    curr[j] = (uchar)cornerScore(ptr, pixel, threshold);
                break;
              }
            }
            else
              count = 0;
          }
        }

        if( d & 2 )
        {
          int vt = v + threshold, count = 0;

          for( k = 0; k < N; k++ )
          {
            int x = ptr[pixel[k]];
            if(x > vt)
            {
              if( ++count > K )
              {
                cornerpos[ncorners++] = j;
                if(m_nonmax_suppression)
                  curr[j] = (uchar)cornerScore(ptr, pixel, threshold);
                break;
              }
            }
            else
              count = 0;
            }
          }
        }
    }

    cornerpos[-1] = ncorners;

    if( i == 3 )
        continue;

    const uchar* prev = buf[(i - 4 + 3)%3];
    const uchar* pprev = buf[(i - 5 + 3)%3];
    cornerpos = cpbuf[(i - 4 + 3)%3];
    ncorners = cornerpos[-1];

    for( k = 0; k < ncorners; k++ )
    {
      j = cornerpos[k];
      int score = prev[j];
      if(!m_nonmax_suppression ||
         (score > prev[j+1] && score > prev[j-1] &&
          score > pprev[j-1] && score > pprev[j] && score > pprev[j+1] &&
          score > curr[j-1] && score > curr[j] && score > curr[j+1]) )
      {
        points.push_back(Point2f((float)j, (float)(i-1)));
      }
    }
  }

  m_stat->add_statistics(FEATURE_DETECTOR, "FAST Detector current time: ", (double) (clock() - begin_time) / CLOCKS_PER_SEC);
  m_stat->add_statistics(FEATURE_DETECTOR, "FAST Detector average time: ", (double) (clock() - begin_time) / CLOCKS_PER_SEC, true);

  return true;
}

OpencvFastFeatureDetector::~OpencvFastFeatureDetector()
{

}

void OpencvFastFeatureDetector::reset()
{
  m_detector.reset();
}

