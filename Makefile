.PHONY: clean All

All:
	@echo "----------Building project:[ SoX_API - Debug ]----------"
	@cd "SoX_API" && "$(MAKE)" -f  "SoX_API.mk"
	@echo "----------Building project:[ NanoCom - Debug ]----------"
	@cd "NanoCom" && "$(MAKE)" -f  "NanoCom.mk"
	@echo "----------Building project:[ Main - Debug ]----------"
	@cd "Main" && "$(MAKE)" -f  "Main.mk"
clean:
	@echo "----------Cleaning project:[ SoX_API - Debug ]----------"
	@cd "SoX_API" && "$(MAKE)" -f  "SoX_API.mk"  clean
	@echo "----------Cleaning project:[ NanoCom - Debug ]----------"
	@cd "NanoCom" && "$(MAKE)" -f  "NanoCom.mk"  clean
	@echo "----------Cleaning project:[ Main - Debug ]----------"
	@cd "Main" && "$(MAKE)" -f  "Main.mk" clean
