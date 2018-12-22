FROM gcc:7
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
CMD make test
