# S3 MAP file layout

A Siedler 3 map file is formed by multiple partitions, each containing distinct information.<br>
Each block of data consists of one byte.

```shell
[16:42:07] Logic: loading map file: assets/maps/s3\4Circles.map
[16:42:07] Logic:       partition : type 1210, length 1870, offset 16
[16:42:07] Logic:       partition : type 1, length 24, offset 1894
[16:42:07] Logic:       partition : type 2, length 180, offset 1926
[16:42:07] Logic:       partition : type 3, length 41, offset 2114
[16:42:07] Logic:       partition : type 256, length 4, offset 2163
[16:42:07] Logic:       partition : type 650, length 3046, offset 2175
[16:42:07] Logic:       partition : type 743, length 3046, offset 5229
[16:42:07] Logic:       partition : type 4, length 131076, offset 8283
[16:42:07] Logic:       partition : type 6, length 1204228, offset 139367
[16:42:07] Logic:       partition : type 1246, length 1469, offset 1343603
[16:42:07] Logic:       partition : type 586, length 1468, offset 1345080
[16:42:07] Logic:       partition : type 0, length 0, offset 1346556
```

The known partition types are:

* [1] Map Info
  * is campaign
  * player count
  * resource preset
* [2] Player Info
* [3] Team Info
* [4] Preview
* [6] Area
  * size
  * nodes[size] { height, type, object, player claim, accessible, resource } 
* [7] Settlers
* [8] Buildings
* [9] Resources
* [11] Quest Text (campaign maps)
* [12] Quest Tip (campaign maps)