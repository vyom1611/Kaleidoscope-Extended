clang++ -g src/MainD.cpp src/AST.cpp src/Codegen.cpp src/Lexer.cpp src/DebugInfo.cpp src/Parser.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core orcjit native` -O3 -o toy

Example workflow:

mkdir build
cd build
cmake ..
make
./toy

Input: ready> def foo( a b c) a * b + c * 2;
Output: Read function definition:define double @foo(double %a, double %b, double %c) {
    entry:
      %c3 = alloca double, align 8
      %b2 = alloca double, align 8
      %a1 = alloca double, align 8
      store double %a, ptr %a1, align 8
      store double %b, ptr %b2, align 8
      store double %c, ptr %c3, align 8
      %a4 = load double, ptr %a1, align 8
      %b5 = load double, ptr %b2, align 8
      %multmp = fmul double %a4, %b5
      %c6 = load double, ptr %c3, align 8
      %multmp7 = fmul double %c6, 2.000000e+00
      %addtmp = fadd double %multmp, %multmp7
      ret double %addtmp
    }

    After SimplifyCFGPass:
    ; ModuleID = 'my cool jit'
    source_filename = "my cool jit"

    define double @foo(double %a, double %b, double %c) {
    entry:
      %c3 = alloca double, align 8
      %b2 = alloca double, align 8
      %a1 = alloca double, align 8
      store double %a, ptr %a1, align 8
      store double %b, ptr %b2, align 8
      store double %c, ptr %c3, align 8
      %a4 = load double, ptr %a1, align 8
      %b5 = load double, ptr %b2, align 8
      %multmp = fmul double %a4, %b5
      %c6 = load double, ptr %c3, align 8
      %multmp7 = fmul double %c6, 2.000000e+00
      %addtmp = fadd double %multmp, %multmp7
      ret double %addtmp
    }

    After InstCombinePass:
    ; ModuleID = 'my cool jit'
    source_filename = "my cool jit"

    define double @foo(double %a, double %b, double %c) {
    entry:
      %multmp = fmul double %a, %b
      %multmp7 = fmul double %c, 2.000000e+00
      %addtmp = fadd double %multmp, %multmp7
      ret double %addtmp
    }

    After ReassociatePass:
    ; ModuleID = 'my cool jit'
    source_filename = "my cool jit"

    define double @foo(double %a, double %b, double %c) {
    entry:
      %multmp = fmul double %a, %b
      %multmp7 = fmul double %c, 2.000000e+00
      %addtmp = fadd double %multmp, %multmp7
      ret double %addtmp
    }

    After NewGVNPass:
    ; ModuleID = 'my cool jit'
    source_filename = "my cool jit"

    define double @foo(double %a, double %b, double %c) {
    entry:
      %multmp = fmul double %a, %b
      %multmp7 = fmul double %c, 2.000000e+00
      %addtmp = fadd double %multmp, %multmp7
      ret double %addtmp
    }

    Optimized function definition:define double @foo(double %a, double %b, double %c) {
    entry:
      %multmp = fmul double %a, %b
      %multmp7 = fmul double %c, 2.000000e+00
      %addtmp = fadd double %multmp, %multmp7
      ret double %addtmp
    }