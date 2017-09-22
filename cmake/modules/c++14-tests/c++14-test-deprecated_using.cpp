
template <class C> class Foo;

template <class C> using Bar [[deprecated]] = Foo<C>;

int main() {
    return 0;
}

