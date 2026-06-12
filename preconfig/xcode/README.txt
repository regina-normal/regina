Xcode configuration
-------------------

On the Regina project info tab in Xcode, you can choose between different
configurations to customise your build.  The options include:

- development.xcconfig, to be used for general day-to-day development;

- release.xcconfig, to be used when building a non-sandboxed release for
  distribution through the Regina website;

- sandbox.xcconfig, to be used when building a sandboxed release for
  distribution through the App Store.

In addition, you can customise tools.xcconfig if you need to specify custom
locations for helper tools and libraries (e.g., the location of doxygen).

 -- Ben Burton <bab@debian.org>, Fri, 12 Jun 2026 16:51:17 +1000
