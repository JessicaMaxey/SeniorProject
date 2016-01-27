.PHONY: clean All

All:
	@echo "----------Building project:[ NanoCom - Debug ]----------"
	@cd "NanoCom" && "$(MAKE)" -f  "NanoCom.mk"
clean:
	@echo "----------Cleaning project:[ NanoCom - Debug ]----------"
	@cd "NanoCom" && "$(MAKE)" -f  "NanoCom.mk" clean
