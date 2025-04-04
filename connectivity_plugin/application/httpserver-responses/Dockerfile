# Copyright (c) 2018-2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.  
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this 
# file except in compliance with the License. You may obtain a copy of the License at
# http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software distributed under the
# License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
# either express or implied. 
# See the License for the specific language governing permissions and limitations under the License.

ARG APAMA_VERSION=26
ARG APAMA_BUILDER=public.ecr.aws/apama/apama-builder:${APAMA_VERSION}
ARG APAMA_IMAGE=public.ecr.aws/apama/apama-correlator:${APAMA_VERSION}

# Use the build environment
FROM ${APAMA_BUILDER} as builder

# Copy project assets into build environment
COPY --chown=1724:1724 . ${APAMA_WORK}/Project

# Deploy the apama project
RUN engine_deploy --outputDeployDir ${APAMA_WORK}/Project_deployed ${APAMA_WORK}/Project

# Validate the project by running tests. Failed tests will fail the image build
RUN cd ${APAMA_WORK}/Project_deployed/tests && pysys run

# Final image is based on the runtime base image
FROM ${APAMA_IMAGE}

# Copy deployed project from the build environment
COPY --chown=1724:1724 --from=builder ${APAMA_WORK}/Project_deployed ${APAMA_WORK}/Project_deployed

WORKDIR ${APAMA_WORK}

# Expose the port for the HTTP server
EXPOSE 8080

# Run the deployed project
CMD ["correlator", "--config", "Project_deployed", "--logfile", "/dev/stdout"]

