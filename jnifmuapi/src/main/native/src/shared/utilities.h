//
// Created by Casper Thule on 22/06/2021.
//

#ifndef FMUAPI_UTILITIES_H
#define FMUAPI_UTILITIES_H

#define DECLARE_ARRAY(type, name, length)             \
  type *name = (type *)malloc(sizeof(type) * length); \
  if (name == NULL) {                                 \
    throwException(env, "malloc r failed");           \
  }

#endif //FMUAPI_UTILITIES_H
