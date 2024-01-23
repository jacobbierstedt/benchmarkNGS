FROM gcc:13.2.0

RUN mkdir benchmarkNGS
COPY . ./benchmarkNGS

WORKDIR /benchmarkNGS
RUN make
WORKDIR ..
