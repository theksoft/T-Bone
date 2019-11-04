# T-Bone - [Global Platform (GP)][1] TEE functional simulator

This project aimed at creating a Global Platform TEE simulator for trusted application functional development.
It offers a convenient way to simulate rich/trusted environement interfacing and isolation within a desktop environment for functional development purpose.

This project **is not**:
+ a [Global Platform (GP)][1]  TEE compliant embedded implementation
+ a [Global Platform (GP)][1]  REE/TEE real-time compliant implementation
+ a trusted OS implentation

## Getting started

> TODO: An Hello world! example

## Overview

T-Bone system is composed of 3 main elements:
+ A TEE simulator server managing one or more trusted application which must be implemented as dynamic libraries
+ A client application library to be linked with your rich application (RA) and connecting to one or more TEE simulator(s).
+ A TEE internal library for simulating TEE internal core API offered to trusted application to be linked with your trusted application (TA).

## Building T-Bone trusted application

> TODO: T-Bone TEE internal library (libtbtee)

## Configuring T-Bone TEE

> TODO: TEE simulation server (tbone)

## Building T-Bone client application

> TODO: T-Bone REE client application library (libtbapp)

## [Global Platform (GP)][1] TEE documentation

[Global Platform Technical Library][2]

+ [Introduction to Trusted Execution Environment][3]
+ [TEE System Architecture][4]
+ [TEE Client API Specification][5] and [Errata][6]
+ [TEE Internal Core API Specification][7]

[1]: <https://globalplatform.org/> "GP Website"
[2]: <https://globalplatform.org/specs-library/> "GP Technical Library"
[3]: <https://globalplatform.org/wp-content/uploads/2018/05/Introduction-to-Trusted-Execution-Environment-15May2018.pdf> "GP TEE introduction"
[4]: <https://globalplatform.org/specs-library/tee-system-architecture-v1-2/> "GP TEE System Architecture v1.2"
[5]: <https://globalplatform.org/specs-library/tee-client-api-specification/> "GP Client API Specification v1.0"
[6]: <https://globalplatform.org/specs-library/tee-client-api-specification-v1-0-errata-and-precisions-v2/> "GP Client API SPecification Errata and Precision v2.0"
[7]: <https://globalplatform.org/specs-library/tee-internal-core-api-specification-v1-2/> "GP TEE Internal Core API Specification v1.2.1"