#include <jni.h>

#ifndef _Included_model_MotorOrdenacao
#define _Included_model_MotorOrdenacao
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_model_MotorOrdenacao_init
  (JNIEnv *, jobject);

JNIEXPORT void JNICALL Java_model_MotorOrdenacao_ordenar
  (JNIEnv *, jobject, jintArray, jstring);

JNIEXPORT void JNICALL Java_model_MotorOrdenacao_cleanup
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif