.PHONY: clean All

All:
	@echo "----------Building project:[ VLC_media_test - Debug ]----------"
	@cd "VLC_media_test" && "$(MAKE)" -f  "VLC_media_test.mk"
clean:
	@echo "----------Cleaning project:[ VLC_media_test - Debug ]----------"
	@cd "VLC_media_test" && "$(MAKE)" -f  "VLC_media_test.mk" clean
