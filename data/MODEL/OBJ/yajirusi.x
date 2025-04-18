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
 31;
 -20.71311;-6.14593;-37.78454;,
 20.71311;-6.14593;-37.78454;,
 20.71311;5.96663;-38.10171;,
 -20.71311;5.96663;-38.10171;,
 0.00000;-1.17407;-79.35499;,
 -20.71311;5.96663;-38.10171;,
 20.71311;5.96663;-38.10171;,
 20.71311;5.96663;-38.10171;,
 20.71311;-6.14593;-37.78454;,
 -20.71311;-6.14593;-37.78454;,
 -20.71311;5.96663;-38.10171;,
 -9.32669;6.74147;-0.22592;,
 9.32669;6.74147;-0.22592;,
 9.32669;5.73531;-38.64856;,
 -9.32669;5.73531;-38.64856;,
 9.32669;6.74147;-0.22592;,
 9.32669;-4.93743;0.07990;,
 9.32669;-5.94356;-38.34271;,
 9.32669;5.73531;-38.64856;,
 9.32669;-4.93743;0.07990;,
 -9.32669;-4.93743;0.07990;,
 -9.32669;-5.94356;-38.34271;,
 9.32669;-5.94356;-38.34271;,
 -9.32669;-4.93743;0.07990;,
 -9.32669;6.74147;-0.22592;,
 -9.32669;5.73531;-38.64856;,
 -9.32669;-5.94356;-38.34271;,
 9.32669;6.74147;-0.22592;,
 -9.32669;6.74147;-0.22592;,
 -9.32669;5.73531;-38.64856;,
 9.32669;5.73531;-38.64856;;
 
 11;
 4;0,1,2,3;,
 3;4,5,6;,
 3;4,7,1;,
 3;4,8,9;,
 3;4,0,10;,
 4;11,12,13,14;,
 4;15,16,17,18;,
 4;19,20,21,22;,
 4;23,24,25,26;,
 4;23,16,27,28;,
 4;29,30,17,26;;
 
 MeshMaterialList {
  1;
  11;
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
   0.800000;0.135200;0.668000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  11;
  0.000000;0.026176;0.999657;,
  0.000000;0.985348;-0.170558;,
  0.894427;-0.011706;-0.447062;,
  0.000000;-0.992924;-0.118755;,
  -0.894427;-0.011706;-0.447062;,
  0.000000;0.999657;-0.026178;,
  1.000000;0.000000;0.000000;,
  0.000000;-0.999657;0.026177;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.026177;0.999657;,
  0.000000;-0.026180;-0.999657;;
  11;
  4;0,0,0,0;,
  3;1,1,1;,
  3;2,2,2;,
  3;3,3,3;,
  3;4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;;
 }
 MeshTextureCoords {
  31;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
