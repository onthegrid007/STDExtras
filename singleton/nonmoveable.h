#ifndef NONMOVEABLE_H_
#define NONMOVEABLE_H_
namespace std {
  class NonMovable {
    public:
    NonMovable(NonMovable &&) = delete;
    NonMovable &operator=(NonMovable &&) = delete;

    protected:
    NonMovable() = default;
  };
};
#endif