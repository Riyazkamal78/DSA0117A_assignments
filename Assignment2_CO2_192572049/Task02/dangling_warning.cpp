int& bad(){int local=42;return local;}
int main(){/* Deliberately do not call bad: a returned local reference dangles. */}
