#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#define GET_SET(type, name)  \
    const type &name() const \
    {                        \
        return _##name;      \
    }                        \
    type &name()             \
    {                        \
        return _##name;      \
    }                        \
    void name(type value)    \
    {                        \
        _##name = value;     \
    }

#endif /* __COMMON_HPP__ */
