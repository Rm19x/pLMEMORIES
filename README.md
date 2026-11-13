# pLMEMORIES 


`pLMEMORIES` adalah D3D MENU khusus untuk antarmuka mesin game taktis dan pengembangan perangkat lunak kustom. Dilengkapi dengan **Sistem Multi-Game Hub** yang memuat **100 Fitur VVIP Lost Saga** dan **100 Fitur VVIP Point Blank** secara lengkap dalam satu kesatuan kode.





##  Panduan Kompilasi & Build

Kamu dapat mengkompilasi `pLMEMORIES` di sistem operasi Windows menggunakan kompiler GCC (MinGW) atau MSVC:

### 1. Menggunakan MinGW (GCC):
Buka terminal (CMD / PowerShell) di direktori file, lalu ketik perintah berikut:
```bash
gcc pLMEMORIES.c -o pLMEMORIES.exe -ld3d9 -lgdi32 -luser32
```

### 2. Menggunakan Visual Studio (Developer Command Prompt):
```cmd
cl pLMEMORIES.c /link d3d9.lib gdi32.lib user32.lib
```


