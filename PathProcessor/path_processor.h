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
    submodule_type m_running_type;

    abstract_feature_tracker *m_feature_tracker;
    abstract_feature_detector *m_feature_detector;
    abstract_video_reader *m_video_reader;

  public:
    path_processor(abstract_feature_tracker *feature_tracker,
                abstract_feature_detector *feature_detector,
                abstract_video_reader *video_reader,
                submodule_type running_type = UNKNOWN);
    ~path_processor();

    void start();
    void pause();
  };
}

#endif // PATH_PROCESSOR_H
