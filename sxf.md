# test.sxf



## Bookmarks

### <span style="background-color: #7329CC80">MAGIC NUMBER [0x0000 - 0x0003]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000000  57 42 53 43                                      WBSC
```

### <span style="background-color: #CC612980">VERSION? [0x0004 - 0x0007]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000000              01 00 00 07                               ....
```

### <span style="background-color: #73CC2980">#1 Sheet Property [0x0008 - 0x0009]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000000                           FF 01                            ..
```

### <span style="background-color: #2EDC2C80">Property SIZE [0x000A - 0x000D]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000000                                 00 00 00 54                  ...T
```

### <span style="background-color: #2932CC80">Total Frames [0x0010 - 0x0013]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000010  00 00 00 67                                      ...g
```

### <span style="background-color: #CC294580">layers [0x0014 - 0x0017]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000010              00 00 00 04                               ....
```

### <span style="background-color: #CC7D2980">fps [0x0018 - 0x0019]</span>

> Default = 24

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000010                           00 18                            ..
```

### <span style="background-color: #CCB52980">Cut [0x0022 - 0x0025]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000020        00 00 00 01                                  ....
```

### <span style="background-color: #29CC3280">Scene [0x001A - 0x001D]</span>

> Default = 1

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000010                                 00 00 00 01                  ....
```

### <span style="background-color: #2986CC80">Indicator Type [0x0032 - 0x0033]</span>

> 1 = Frame
> 2 = Inch
> 4 = Page + Frame
> 8 = Second + Frame

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000030        00 01                                        ..
```

### <span style="background-color: #29CC4E80">Frames Per Page [0x0036 - 0x0037]</span>

> Short Int I guess

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000030                    00 90                                 ..
```

### <span style="background-color: #4296F980">WIDGETS-NO NEED TO EDIT [0x0038 - 0x0045]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000030                           00 06 00 01 00 04 00 08          ........
00000040  00 02 00 10 00 19                                ......
```

### <span style="background-color: #CC452980">Refer Line Gap [0x0034 - 0x0035]</span>

> Defualt = 6
> Short Int

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000030              00 06                                    ..
```

### <span style="background-color: #29CC3280">Sheet SIZE [0x0070 - 0x0073]</span>

> =(Frames×10×Columns)+60(十进制)

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000070  00 00 10 54                                      ...T
```

### <span style="background-color: #CC7D2980">Column SIZE [0x0074 - 0x0077]</span>

> =(Frames×10)+11(十进制)
> 11bytes应该是列头？这样可以避免误读FF标签（这是唯一可能出现FF的地方）

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000070              00 00 04 11                               ....
```

### <span style="background-color: #4029CC80">Show ACTION [0x0046 - 0x0049]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000040                    00 00  00 01                          ....
```

### <span style="background-color: #CC299980">Show CELL [0x004A - 0x004D]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000040                                 00 00 00 01                  ....
```

### <span style="background-color: #2986CC80">Show DIALOG [0x004E - 0x0051]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000040                                             00 00                ..
00000050  00 01                                            ..
```

### <span style="background-color: #C7CC2980">Show SOUND [0x0052 - 0x0055]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000050        00 00 00 00                                  ....
```

### <span style="background-color: #29CC4E80">Show CAM [0x0056 - 0x0059]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000050                    00 00  00 01                          ....
```

### <span style="background-color: #CC992980">Show DIRECT [0x005A - 0x005D]</span>

> 指示内容

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000050                                 00 00 00 01                  ....
```

### <span style="background-color: #5929CC80">Show Basic Info [0x005E - 0x0061]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000050                                             00 00                ..
00000060  00 01                                            ..
```

### <span style="background-color: #CC612980">#2 DIRECT CONTENT [0x0062 - 0x0063]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000060        FF 02                                        ..
```

### <span style="background-color: #29CC3280">Big Font DIRECT [0x006A - 0x006D]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000060                                 00 00 00 00                  ....
```

### <span style="background-color: #BEEEDA80">#3 ACTION [0x006E - 0x006F]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000060                                             FF 03                ..
```

### <span style="background-color: #F8C62580">CONTENT SIZE [0x0068 - 0x0069]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000060                           00 00                            ..
```

### <span style="background-color: #4296F980">DIRECT SIZE [0x0064 - 0x0067]</span>

> 

```
Hex View  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F
 
00000060              00 00 00 06                               ....
```



## Data description

| Type | Value |
| ---- | ----- |
| 路径 | E:\Abel\Documents\sxfViewer\build\Desktop_Qt_5_15_2_MSVC2019_64bit-Debug\qtc_Visual_Studio_17_2022\test.sxf |
| 大小 | 8.92 kiB |
| 创建时间 | 2025-11-15 11:33:39 |
| 最后访问时间 | 2026-03-15 20:29:05 |
| 最后更改时间 | 2025-11-15 19:59:29 |






