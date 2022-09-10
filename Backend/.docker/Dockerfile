# syntax=docker/dockerfile:1
# version de golang
FROM golang:1.16-alpine
#espacio de trabajo
WORKDIR /app
#modulo dentro de la imagen
COPY go.mod ./
COPY go.sum ./
#ejecuto el go
RUN go mod download
#copia todos los archivos go
COPY *.go ./
EXPOSE 3000
CMD [ "go", "run", "main.go" ]