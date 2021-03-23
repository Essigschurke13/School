openssl_task: openssl_task.c
	mkdir -p ./build
	gcc $^ -o ./build/$@ -lssl -lcrypto

clean:
	rm -rdf ./build

.PHONY: clean