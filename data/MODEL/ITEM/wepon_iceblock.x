xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 23;
 -0.08903;13.13452;-0.66071;,
 5.33952;8.98954;-3.20640;,
 0.00000;11.04638;-3.45953;,
 -0.08903;13.13452;-0.66071;,
 -1.77531;10.88594;4.76784;,
 -0.08903;13.13452;-0.66071;,
 -5.42855;8.66023;-0.00000;,
 -0.08903;13.13452;-0.66071;,
 0.00000;11.04638;-3.45953;,
 7.67713;5.61783;0.00000;,
 0.00000;5.61783;-7.67713;,
 -0.00000;5.61783;7.67713;,
 -7.67713;5.61783;-0.00000;,
 0.00000;5.61783;-7.67713;,
 6.39649;0.58566;-0.62733;,
 0.00000;1.25301;-8.21233;,
 -0.00000;1.06910;8.21233;,
 -7.00626;0.18929;-0.00000;,
 0.00000;1.25301;-8.21233;,
 0.00000;-0.02815;-0.00000;,
 0.00000;-0.02815;-0.00000;,
 0.00000;-0.02815;-0.00000;,
 0.00000;-0.02815;-0.00000;;
 
 16;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 4;2,1,9,10;,
 4;1,4,11,9;,
 4;4,6,12,11;,
 4;6,8,13,12;,
 4;10,9,14,15;,
 4;9,11,16,14;,
 4;11,12,17,16;,
 4;12,13,18,17;,
 3;15,14,19;,
 3;14,16,20;,
 3;16,17,21;,
 3;17,18,22;;
 
 MeshMaterialList {
  2;
  16;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.409020;0.700000;0.656078;1.000000;;
   7.500000;
   0.680000;0.680000;0.680000;;
   0.058431;0.100000;0.093725;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  21;
  -0.153958;0.789597;-0.593998;,
  -0.108775;0.641381;-0.759472;,
  0.450887;0.523553;-0.722906;,
  0.632889;0.618212;0.466117;,
  -0.763884;0.635880;-0.110170;,
  0.646203;0.113397;-0.754694;,
  0.736518;0.295415;0.608500;,
  -0.742584;0.131316;0.656754;,
  0.738230;-0.006583;-0.674517;,
  0.759279;-0.096847;0.643518;,
  -0.736602;-0.005754;0.676302;,
  0.031771;-0.999434;-0.011057;,
  0.665462;0.593575;0.452581;,
  -0.628419;0.768097;0.122949;,
  -0.729982;0.553871;0.400441;,
  -0.699973;0.242613;-0.671697;,
  -0.737142;-0.004239;-0.675725;,
  0.024435;-0.987754;-0.154095;,
  0.094437;-0.995469;-0.011108;,
  0.038458;-0.990459;0.132336;,
  -0.031019;-0.999459;-0.010962;;
  16;
  3;0,2,1;,
  3;12,3,3;,
  3;13,4,14;,
  3;0,1,4;,
  4;1,2,5,5;,
  4;3,3,6,6;,
  4;14,4,7,7;,
  4;4,1,15,15;,
  4;5,5,8,8;,
  4;6,6,9,9;,
  4;7,7,10,10;,
  4;15,15,16,16;,
  3;17,18,11;,
  3;18,19,11;,
  3;19,20,11;,
  3;20,17,11;;
 }
 MeshTextureCoords {
  23;
  0.125000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.375000;0.000000;,
  0.500000;0.250000;,
  0.625000;0.000000;,
  0.750000;0.250000;,
  0.875000;0.000000;,
  1.000000;0.250000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.125000;1.000000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.875000;1.000000;;
 }
}
