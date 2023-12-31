**CppNext** is an experimental compiler for a potential CppNext syntax.  Currently it exists to transpile CppNext (.cppn) files to **standard, human readable Cpp files**. This compiler is a **work in progress**, and all support is welcome (evne if you just want to hang out and chat :-) )!

## Goals: 
CppNext aims to offer a transformative, but very familiar syntax to existing Cpp developers, while "fixing" some of the more annoying footguns in the language and providing a ramp for increased safety and better error messages.

## Aims:
None of the listed below targets are set in stone.  More may be added, some may be removed or changed.

 1. 100% interoperability with existing Cpp codebases, including support for templates and normal Cpp.
 2. Pure compile time programming made easy:  Compile time programming is done in normal CppNext.
 3. Implement a borrow checker along with safe/unsafe blocks.
 4. Package Manager and cross platform build system standardized.  It's 2023.
 5. Const by default.  CppNext will introduce the new keyword mut to change a value to mutable.
 6. No Raw Pointers as Cpp defines them: all pointers in CppNext are smart unique pointers by default.
 7. Arrays are smart arrays: They bundle their size with them along with standard functions.
 8. Assignment operators are narrowed to remove The Most Vexing Parse.  This also makes it much easier to teach and mentally reason about CppNext.
 9.  Iostream is not the default.  The fmt library is the default for output, and other mechanisms are provided for cin.
 10. Return values must be used by default.
 11. Errors returned via Result.
 12. Easy Multi-return values.
 13. Parameter binding.
 14. 0 != NULL.  Long live null
 15. UTF8 by default.
 16. All Three major platforms (windows, mac, linux) will be treated as first class targets.

## Structure and Leadership:
An inclusive space, where all are welcome.  For now it's just a one-person show, but if you feel interested in participating, greetings!  All communication/decisions will be in the public space.  For now, we'll adopt the Linux Contributor Code of Conduct.
