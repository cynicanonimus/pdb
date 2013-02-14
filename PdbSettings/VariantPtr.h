#ifndef VARIANTPTR_H
#define VARIANTPTR_H
//
// VariantPtr.h
//
#include <QVariant>
//
template <class T> class VariantPtr
{
public:
    static T* asPtr(QVariant v)
    {
    return  (T *) v.value<void *>();
    }

    static QVariant asQVariant(T* ptr)
    {
    return qVariantFromValue((void *) ptr);
    }
};

#endif // VARIANTPTR_H
