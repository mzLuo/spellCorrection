#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H
namespace wd
{

class Noncopyable
{
protected:
    Noncopyable(){}
    ~Noncopyable(){}

private:
    Noncopyable(const Noncopyable &);
    Noncopyable &operator=(const Noncopyable &);
};

} // end of namespace wd
#endif// end _NONCOPYABLE_H
