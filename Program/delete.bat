del /f /s /q \debug\*.*
rd /s /q debug
del /f /s /q ipch\*.*
rd /s/q  ipch
del /f /s /q \.vs\*.*
rd /s /q .vs
del /s /q *.sdf 
del /f /s /q \x64\debug\*.*
rd /s /q x64\debug
rd /s /q x64
del /f /s /q \test\x64\debug\*.*
rd /s /q test\x64\debug
del /f /s /q \test\debug\*.*
rd /s /q test\debug
del /f /s /q test\ipch\*.*
rd /s/q  test\ipch
del /f /s /q \test\.vs\*.*
rd /s /q test\.vs
del /s /q test\*.sdf 

