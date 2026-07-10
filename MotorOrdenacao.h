#include <jni.h>

#ifndef _Included_MotorOrdenacao
#define _Included_MotorOrdenacao
#ifdef __cplusplus
extern "C" {
#endif

// Chamado quando o usuário clica em "Iniciar" ou "Gerar Vetor" no Java
JNIEXPORT void JNICALL Java_controller_OrdenacaoController_inicializarMotorNativo
  (JNIEnv *, jobject, jintArray, jstring);

// Chamado se o Java precisar encerrar explicitamente o motor
JNIEXPORT void JNICALL Java_controller_OrdenacaoController_encerrarMotorNativo
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif