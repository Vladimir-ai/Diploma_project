#ifndef PATH_PROCESSOR_H
#define PATH_PROCESSOR_H

#include <opencv2/core/mat.hpp>
#include <Eigen/Dense>
#include <vector>
#include <FeatureTracker/feature_tracker.h>
#include <VideoReader/video_reader.h>
#include <FeatureDetector/feature_detector.h>

using namespace cv;
using namespace std;
using namespace submodule_video_reader;
using namespace submodule_feature_detector;
using namespace submodule_feature_tracker;

/* Use 100 interval */
enum submodule_type
{
  UNKNOWN = 0,
  /* Video reader start */
  OPENCV_READER = 100,
  /* Video reader end */

  /* Feature detector start */
  FAST_DETECTOR = 200,
  /* Feature detector end */

  /* Feature tracker start */
  NOT_IMPLEMENTED = 300,
  /* Feature tracker end */

  /* Running mode */
  STEP_BY_STEP = 400,
  CONTINUOUSLY = 401
};


namespace module_path_processor
{
  /* It should only run file or sth with selected features */
  class path_processor
  {
  protected:
    Eigen::MatrixXi m_current_img;

    bool m_is_running = false;
    submodule_type m_running_type;

    abstract_feature_tracker *m_feature_tracker;
    abstract_feature_detector *m_feature_detector;
    abstract_video_reader *m_video_reader;

  public:
    path_processor();

    path_processor(abstract_feature_tracker *feature_tracker,
                abstract_feature_detector *feature_detector,
                abstract_video_reader *video_reader);

    void set_feature_tracker(abstract_feature_tracker *feature_tracker);
    void set_feature_detector(abstract_feature_detector *feature_detector);
    void set_video_reader(abstract_video_reader *video_reader);

    ~path_processor();

    Eigen::MatrixXi get_curr_frame(void);

    bool can_start(void);
    void start(submodule_type running_type);
    void pause();
  };

  abstract_feature_detector *construct_feature_detector(submodule_type detector_type);
  abstract_feature_tracker *construct_feature_tracker(submodule_type tracker_type);
  abstract_video_reader *construct_video_reader(submodule_type reader_type, std::string path = "");
}

#endif // PATH_PROCESSOR_H
