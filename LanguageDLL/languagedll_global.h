#ifndef LANGUAGEDLL_GLOBAL_H
#define LANGUAGEDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LANGUAGEDLL_LIBRARY)
#  define LANGUAGEDLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LANGUAGEDLLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LANGUAGEDLL_GLOBAL_H
