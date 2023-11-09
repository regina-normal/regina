Swift/C++ Glue
--------------

These headers provide glue to make packet-related classes and functions
available to Swift, despite the limitations of Swift/C++ interoperability.

Many of the workarounds in these headers are to address the limitations of
Swift support for raw pointers and std::shared_ptr<...>:

- The pointee property of a std::shared_ptr<...> appears to return a deep copy
  of the pointee.  Thus it cannot be used with:

  * modifying functions, such as Text::setText(); or

  * functions that return internal references, such as Text::text() which
    returns a (const std::string&); or

  * abstract base classes in any way, such as Packet.

- Crossings, faces and simplices in Regina are passed around by pointer (with
  no need for users to manage their lifespans).  These classes are not
  accessible through Swift at all, since they are non-copyable.

It is entirely possible that the Swift/C++ has clean solutions to these
problems, in which case Ben would _really_ love an email to this effect.

These headers and workarounds were written in late 2023, after the release of
Swift 5.9 (the first to formally support Swift/C++ interop).

  -- Ben Burton <bab@debian.org>, 5 November 2023

