.PHONY: default generate

generate:
	@cd messages && python3 ./nanopdb/generator/nanopb_generator.py messages.proto --output-dir=../src