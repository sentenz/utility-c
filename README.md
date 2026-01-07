# utility-c library

A collection of utility functions in c.

- [1. Setup](#1-setup)
  - [1.1. Git Credential Manager](#11-git-credential-manager)
    - [1.1.1. useHttpPath](#111-usehttppath)
    - [safe.directory](#safedirectory)

## 1. Setup

### 1.1. Git Credential Manager

Secure [Git credential](http://microsoft.github.io/Git-Credential-Manager-for-Windows/Docs/Configuration.html) storage for Windows with support for Visual Studio Team Services, GitHub, and Bitbucket multi-factor authentication.

#### 1.1.1. useHttpPath

Instructs Git to supply the path portion of the remote URL to credential helpers. When path is supplied, the GCM will use the host-name + path as the key when reading and/or writing credentials.

NOTE This option changes the behavior of Git.

Supports true or false. Defaults to false.

```bash
git config --global credential.https://dev.azure.com.useHttpPath true
```

#### safe.directory

Supports true or false. Defaults to false.

```bash
git config --global --add safe.directory <path-to-project>
```
