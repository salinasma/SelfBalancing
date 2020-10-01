
build_web:
	docker build -t sample:dev .
run_web:
	docker run \
    -it \
    --rm \
    -v ${PWD}:/app \
    -v /app/node_modules \
    -p 3001:3000 \
    -e CHOKIDAR_USEPOLLING=true \
    sample:dev




backend:
	clear; endpoints --prefix=controller --host=localhost:8000
	
