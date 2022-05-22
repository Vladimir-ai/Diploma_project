#ifndef COMMON_H
#define COMMON_H

#ifdef QT_ASSERTION
#include <QtGlobal>
#define ASSERT(cond)    Q_ASSERT(cond)
#endif /* Q_ASSERT */

#include <string>

/* Use 100 interval */
enum submodule_type
{
  UNKNOWN = 0,
  /* General types */
  READER = 1,
  DETECTOR = 2,
  FEATURE_TRACKER = 3,
  ESTIMATOR = 4,

  /* Video reader start */
  DYNAMIC_LOADED_READER = 100,
  OPENCV_READER = 101,
  MAX_READER = 199,
  /* Video reader end */

  /* Feature detector start */
  DYNAMIC_LOADED_DETECTOR = 200,
  FAST_DETECTOR = 201,
  MAX_DETECTOR = 299,
  /* Feature detector end */

  /* Feature tracker start */
  DYNAMIC_LOADED_TRACKER = 300,
  OPENCV_FEATURE_TRACKER = 301,
  MAX_TRACKER = 399,
  /* Feature tracker end */

  /* path estimator start */
  DYNAMIC_LOADED_ESTIMATOR = 400,
  OPENCV_POSE_ESTIMATOR = 401,
  MAX_ESTIMATOR = 499,
  /* path estimator end */

  /* Running mode */
  STEP_BY_STEP = 500,
  CONTINUOUSLY = 501,
};

enum submodule_type convert_to_general_type(enum submodule_type local_type);

typedef void (*func_error_handler)(std::string error);

#endif // COMMON_H
