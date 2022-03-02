# TrackerClass
So, target is to track all constructors of target class T, and get detailed understanding where copy/move constructors called and how much copies created.
Using flag -fno-elide-constructors, we can see additional copies.
Further task is to create examples where std::move and std::forward are useful, and where they are used incorrectly.
