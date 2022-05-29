#ifndef PATH_PROCESSOR_H
#define PATH_PROCESSOR_H

#include <opencv2/core/mat.hpp>
#include <Eigen/Dense>
#include <vector>
#include "Common/common.h"
#include "FeatureTracker/feature_tracker.h"
#include "VideoReader/video_reader.h"
#include "FeatureDetector/feature_detector.h"
#include "PoseEstimator/abstract_pose_estimator.h"
#include "Logger/logger.h"
#include "PathDrawer/pathdrawer.h"

#include <mutex>
#include <condition_variable>


using namespace cv;
using namespace std;
using namespace SubmoduleVideoReader;
using namespace SubmoduleFeatureDetector;
using namespace SubmoduleFeatureTracker;
using namespace SubmodulePoseEstimator;


namespace ModulePathProcessor
{
  /* It should only run file or sth with selected features */
  class PathProcessor
  {
  private:
    std::mutex m_img_mutex;
    std::mutex m_running_mutex;
    std::condition_variable m_cond_variable;

    Mat m_img[2];
    vector<Point2f> m_keypoints[2];
    uint8_t m_current_switch = 0;

    bool m_is_running = false;
    bool m_is_stopped = false;
    submodule_type m_running_type;

    IAbstractFeatureTracker *m_feature_tracker = nullptr;
    IAbstractFeatureDetector *m_feature_detector = nullptr;
    IAbstractVideoReader *m_video_reader = nullptr;
    IAbstractPoseEstimator *m_pose_estimator = nullptr;

    AbstractLogger *m_logger = nullptr;

    PathDrawer *m_path_drawer;
    Statistics *m_stat = nullptr;

  public:
    PathProcessor(IAbstractFeatureTracker *feature_tracker,
                  IAbstractFeatureDetector *feature_detector,
                  IAbstractVideoReader *video_reader,
                  IAbstractPoseEstimator *pose_estimator,
                  PathDrawer *path_drawer,
                  Statistics *stat);

    void set_logger(AbstractLogger *logger);

    void draw_trace(Mat &frame, const vector<Point2f> &src_keypoints, const vector<Point2f> &dst_keypoints);

    ~PathProcessor();

    Mat get_curr_frame(void);

    bool is_started(void);
    bool is_stopped(void);
    void start(submodule_type running_type);
    void step(void);
    void pause(void);
    void continue_running(void);
    void stop(void);

    Mat get_path_as_img(void);

  private:
    void thread_running(uint8_t job_count);
    void process_frame(uint8_t job_count);

//    do thread running.
  };
}

#endif // PATH_PROCESSOR_H
