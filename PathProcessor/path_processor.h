#ifndef PATH_PROCESSOR_H
#define PATH_PROCESSOR_H

#include <opencv2/core/mat.hpp>
#include <Eigen/Dense>
#include <vector>
#include "Common/common.h"
#include "FeatureTracker/feature_tracker.h"
#include "VideoReader/video_reader.h"
#include "FeatureDetector/feature_detector.h"
#include "Logger/logger.h"

#include <mutex>
#include <condition_variable>


using namespace cv;
using namespace std;
using namespace submodule_video_reader;
using namespace submodule_feature_detector;
using namespace submodule_feature_tracker;


namespace module_path_processor
{
  /* It should only run file or sth with selected features */
  class PathProcessor
  {
  protected:
    std::mutex m_img_mutex;
    std::mutex m_running_mutex;
    std::condition_variable m_cond_variable;

    Mat m_img[2];
    vector<Point2f> m_keypoints[2];
    uint8_t m_current_switch = 0;

    bool m_is_running = false;
    bool m_is_stopped = false;
    submodule_type m_running_type;

    abstract_feature_tracker *m_feature_tracker = nullptr;
    abstract_feature_detector *m_feature_detector = nullptr;
    abstract_video_reader *m_video_reader = nullptr;

    abstract_logger *m_logger = nullptr;

  public:
    PathProcessor();

    PathProcessor(abstract_feature_tracker *feature_tracker,
                abstract_feature_detector *feature_detector,
                abstract_video_reader *video_reader);

    void set_feature_tracker(abstract_feature_tracker *feature_tracker);
    void set_feature_detector(abstract_feature_detector *feature_detector);
    void set_video_reader(abstract_video_reader *video_reader);

    void get_feature_tracker(abstract_feature_tracker *feature_tracker);
    void get_feature_detector(abstract_feature_detector *feature_detector);
    void get_video_reader(abstract_video_reader *video_reader);

    void set_logger(abstract_logger *logger);

    void draw_trace(Mat &frame);

    ~PathProcessor();

    Mat get_curr_frame(void);

    bool is_started(void);
    bool is_stopped(void);
    void start(submodule_type running_type);
    void step(void);
    void pause(void);
    void continue_running(void);
    void stop(void);

  private:
    void thread_running(uint8_t job_count);
    void process_frame(uint8_t job_count);

//    do start and pause,
//    do thread running.
  };

  abstract_feature_detector *construct_feature_detector(submodule_type detector_type);
  abstract_feature_tracker *construct_feature_tracker(submodule_type tracker_type);
  abstract_video_reader *construct_video_reader(submodule_type reader_type, std::string path = "");
}

#endif // PATH_PROCESSOR_H
