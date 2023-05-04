FROM nginx

EXPOSE 4242

COPY ./nginx.conf /etc/nginx/nginx.conf

COPY ./index.html /usr/share/nginx/html
COPY ./nginx.conf /etc/nginx/nginx.conf

RUN mkdir /usr/share/nginx/html/hello

COPY ./hello/hello.html /usr/share/nginx/html/hello
