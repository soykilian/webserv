

all:
	docker run -d  -p 4242:4242 --name webserv nginx || (docker stop webserv && docker rm webserv)
