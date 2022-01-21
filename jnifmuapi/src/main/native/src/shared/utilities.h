//
// Created by Casper Thule on 22/06/2021.
//

#ifndef FMUAPI_UTILITIES_H
#define FMUAPI_UTILITIES_H

#ifdef __cplusplus
#define DECLARE_ARRAY(type, name, length)             \
  auto *name = (type *)malloc(sizeof(type) * length); \
  if (name == NULL) {                                 \
    throwException(env, "malloc r failed");           \
  }
#else
#define DECLARE_ARRAY(type, name, length)             \
  type *name = (type *)malloc(sizeof(type) * length); \
  if (name == NULL) {                                 \
    throwException(env, "malloc r failed");           \
  }
#endif



#endif //FMUAPI_UTILITIES_H
