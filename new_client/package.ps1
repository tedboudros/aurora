$LOVE_EXE = "C:\Program Files\LOVE\love.exe"

# Compress the app files
Compress-Archive -Update ./** main.zip

# Rename .zip to .love
Rename-Item -Path main.zip -NewName main.love

# Inject the compressed app to LOVE
cmd /c copy /b "C:\Program Files\LOVE\love.exe"+main.love main.exe

# Delete compressed files
Remove-Item main.love