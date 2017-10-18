# Tugas Besar Jarkom IF3130 : Sliding Window

## Petunjuk Penggunaan

## Cara Kerja Sliding Window

## Pembagian Tugas

## Soal 
1. Apa yang terjadi jika advertised window yang dikirim bernilai 0? Apa cara untuk menangani hal tersebut?
2. Sebutkan field data yang terdapat TCP Header serta ukurannya, ilustrasikan, dan jelaskan kegunaan dari masing-masing field data tersebut!

## Jawaban

1. Jika advertised window bernilai 0, maka data tidak akan bisa lagi diterima. Proses pengiriman diberhentikan sementara dan client pun harus menunggu hingga advertised window tidak berukuran 0. Untunk menangani hal ini, seperti yang telah disebutkan, karena window berukuran 0 maka data tidak akan bisa diterima. Akan tetapi, sender hanya perlu terus terusan berusaha mengirimkan data. Dengan begitu, akan memacu ukuran window yang ditawarkan client yang pada akhirnya ukuran windownya berubah menjadi tidak 0.

2. Terdapat 11 jenis field data TCP Hedaer, yaitu
    1. Source TCP port number (2 bytes)
    2. Destination TCP port number (2 bytes)
    3. Sequence number (4 bytes)
    4. Acknowledgment number (4 bytes)
    5. TCP data offset (4 bits)
    6. Reserved data (3 bits)
    7. Control flags (up to 9 bits)
    8. Window size (2 bytes)
    9. TCP checksum (2 bytes)
    10. Urgent pointer (2 bytes)
    11. TCP optional data (0-40 bytes)
    
    Ke-sebelas field data tersebut diatur sehingga membentuk tatanan field data seperti berikut:
    
    ### Ilustrasi Field Data TCP Header 
    ![](tcpheader.png)
      
