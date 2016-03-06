template<typename T>
  struct class_is_callable
  {
  private:
    typedef char(&yes)[1];
    typedef char(&no)[2];

    //Fallback has an overloaded  operator()
    struct Fallback { void operator()() {}; };
    //Derive from the input type (T) and Fallback
    struct Derived : T, Fallback {};

    template<typename U, U> struct Check;

    template<typename>
    static yes test(...);

    //Checks if the signature of C's operator() is the same as Fallback's operator()
    template<typename C>
    static no test(Check<void (Fallback::*)(), &C::operator()>*);

  public:
    static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
  };

  template<typename T>
  struct is_callable :
    std::conditional_t<std::is_class<T>::value, class_is_callable<T>, std::is_function<T>> {};
