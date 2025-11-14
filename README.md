# SQRL Service Provider API for Linux

*SQRL's Service Provider (SSP) API defines a proven application programming interface to support the externalization of SQRL services from the relying website.*

See https://www.grc.com/sqrl/sspapi.htm for further details.

This is an implementation of the SSP API for Linux written in C.

## Security Notice ⚠️

**Updated November 2025** - This codebase has been updated with comprehensive security improvements:

- ✅ **Secure Memory Handling**: All sensitive data (keys, tokens, authentication data) is now securely cleared from memory before being freed
- ✅ **Updated Dependencies**: All dependencies updated to latest stable/LTS versions with security patches
- ✅ **Compliance**: Implements secure coding standards (CWE-226, CWE-200) for sensitive data handling
- ✅ **Test Suite**: Includes comprehensive security tests for memory handling

See [SECURITY_REVIEW.md](SECURITY_REVIEW.md) for complete details on security improvements.

## Installation

SSP API Installation for 64-bit Linux (Ubuntu 18.04+ LTS):
* Go to your downloads folder:
 `cd ~/Downloads`

* Clone the project's Github repository:
`git clone https://github.com/sqrldev/SSP-API-Linux.git`

* Go into the project dir:
 `cd SSP-API-Linux`

* Open `install.html` for further instructions, or run:
 `./install.sh --default`

## Dependencies

### Updated Versions (November 2025)

This project relies on the following dependencies, all updated to the latest stable versions:

| Dependency | Version | Type | Notes |
|-----------|---------|------|-------|
| **Berkeley DB** | 18.1.40 | Required | Latest stable release (May 2020) |
| **Libsodium** | 1.0.20 | Required | Modern cryptography library (May 2024) |
| **MBedTLS** | 3.6.5 | Required | Long-term support release (LTS until 2027) |
| **OpenSSL** | 3.5.4 | Required | Long-term support release (LTS until 2030) |
| **QRencode** | 4.1.1 | Required | Latest stable release (Sept 2020) |
| **LodePNG** | 20190210 | Bundled | PNG encoding for QR codes |
| **Blowfish** | - | Bundled | By Andrew Carter |
| **XAMPP** | 8.2.12-0 | Optional | PHP 8.2 environment |

**Build Requirements**: gcc, make, **cmake** (new), autoconf, net-tools

Blowfish and LodePNG are bundled with the source code. All other dependencies must be downloaded and installed either using your distro's package manager or manually. The `install.sh` script handles this automatically.

Using XAMPP is required only if you haven't got an Apache/PHP environment set up already.

### Previous Versions (Deprecated)

<details>
<summary>Click to see previous versions (pre-November 2025)</summary>

- libsodium-1.0.18-stable (2019)
- mbedtls-2.16.3-apache (2019)
- openssl-1.1.1d (2019)
- qrencode-4.0.2 (2019)
- Berkeley DB 18.1.32 (2019)
- xampp-linux-x64-7.3.11-0 (PHP 7.3, EOL)

**Note**: These versions are outdated and should not be used for new installations.
</details>

## Security Features

### Secure Memory Handling

The SSP-API now implements comprehensive secure memory handling to prevent sensitive data recovery:

- **Secure Clearing**: All sensitive data (cryptographic keys, authentication tokens, session identifiers) is securely cleared from memory using `explicit_bzero()` or volatile pointer techniques
- **Before & After**: Memory is zero-initialized before use (via `calloc`) and securely cleared after use
- **Prevents Recovery**: Compiler optimization is prevented from removing memory clearing operations

### Testing Security

Run the security test suite to verify secure memory handling:

```bash
cd SSP-API-Source
gcc -o test_secure_memory test_secure_memory.c utils.c -I. -lpthread
./test_secure_memory
```

Expected output: All tests should pass, confirming secure memory clearing is working correctly.

### Compliance

This implementation adheres to:
- **CWE-226**: Sensitive Information Uncleared Before Release
- **CWE-200**: Exposure of Sensitive Information to an Unauthorized Actor
- **CERT C Secure Coding Standard**: MSC06-C, MEM03-C
- **OWASP Secure Coding Practices**

## Documentation

- **[SECURITY_REVIEW.md](SECURITY_REVIEW.md)** - Comprehensive security review and improvements
- **[install.html](install.html)** - Detailed installation instructions
- **[LICENSE](LICENSE)** - MIT Open Source License

## License

This project is licensed under the MIT Open Source License.
For more information, please read the [LICENSE](LICENSE) file.

Please note however, that the dependencies listed above are all subject to their own licensing terms and conditions.

## Contributing

When contributing to this project, please ensure:
1. All sensitive data is cleared using `SecureMemoryClear()` or `SecureGlobalFree()`
2. Memory containing cryptographic material, keys, or authentication tokens is never left in memory after use
3. New code passes security tests
4. Dependencies are kept up to date with security patches

## Version History

### November 2025 - Security & Dependency Update
- Implemented secure memory clearing for all sensitive data
- Updated all dependencies to latest stable/LTS versions
- Added comprehensive security test suite
- Added security documentation
- Updated build system for modern dependencies

### Earlier Versions
- See git history for earlier changes
