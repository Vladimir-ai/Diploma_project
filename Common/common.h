#ifndef COMMON_H
#define COMMON_H

#ifdef QT_ASSERTION
#include <QtGlobal>
#define ASSERT(cond)    Q_ASSERT(cond)
#endif /* Q_ASSERT */

/* Use 100 interval */
enum submodule_type
{
  UNKNOWN = 0,
  /* Video reader start */
  DYNAMIC_LOADED_READER = 100,
  OPENCV_READER = 101,
  /* Video reader end */

  /* Feature detector start */
  DYNAMIC_LOADED_DETECTOR = 200,
  FAST_DETECTOR = 201,
  /* Feature detector end */

  /* Feature tracker start */
  DYNAMIC_LOADED_TRACKER = 300,
  OPENCV_FEATURE_TRACKER = 301,
  /* Feature tracker end */

  /* Running mode */
  STEP_BY_STEP = 400,
  CONTINUOUSLY = 401,
};

#endif // COMMON_H
