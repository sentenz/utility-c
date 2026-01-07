# `/external`

The `external` directory is reserved for embedding of external projects. Each embedded project should occupy a single subdirectory of `external`.

`external` should not contain files other than those required by tooling.

This directory may be automatically populated, either partially or completely, by tools (eg. git submodules) as part of a build process. In this case, projects must declare the auto-populated subdirectories as ignored by relevant source control systems.

Subdirectories of `external` should not be modified as part of regular project development. Subdirectories should remain as close to their upstream source as possible.
