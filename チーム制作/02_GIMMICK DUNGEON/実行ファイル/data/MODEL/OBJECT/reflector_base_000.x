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
 26;
 0.00000;9.55421;-77.66965;,
 54.92070;9.55421;-54.92070;,
 54.92070;0.04191;-54.92070;,
 0.00000;0.04191;-77.66965;,
 77.66965;9.55421;0.00001;,
 77.66965;0.04191;0.00001;,
 54.92070;9.55421;54.92070;,
 54.92070;0.04191;54.92070;,
 -0.00001;9.55421;77.66965;,
 -0.00001;0.04191;77.66965;,
 -54.92073;9.55421;54.92069;,
 -54.92073;0.04191;54.92069;,
 -77.66965;9.55421;0.00000;,
 -77.66965;0.04191;0.00000;,
 -54.92073;9.55421;-54.92070;,
 -54.92073;0.04191;-54.92070;,
 0.00000;9.55421;0.00000;,
 54.92070;9.55421;-54.92070;,
 0.00000;9.55421;-77.66965;,
 77.66965;9.55421;0.00001;,
 54.92070;9.55421;54.92070;,
 -0.00001;9.55421;77.66965;,
 -54.92073;9.55421;54.92069;,
 -77.66965;9.55421;0.00000;,
 -54.92073;9.55421;-54.92070;,
 0.00000;0.04191;0.00000;;
 
 24;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 3;16,17,18;,
 3;16,19,17;,
 3;16,20,19;,
 3;16,21,20;,
 3;16,22,21;,
 3;16,23,22;,
 3;16,24,23;,
 3;16,18,24;,
 3;25,3,2;,
 3;25,2,5;,
 3;25,5,7;,
 3;25,7,9;,
 3;25,9,11;,
 3;25,11,13;,
 3;25,13,15;,
 3;25,15,3;;
 
 MeshMaterialList {
  3;
  24;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\monorisu.jpg";
   }
  }
  Material {
   0.344800;0.184800;0.031200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\reflector_base_000.jpg";
   }
  }
 }
 MeshNormals {
  10;
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.000000;0.000000;1.000000;,
  -0.707107;0.000000;0.707107;,
  -1.000000;0.000000;0.000000;,
  -0.707107;0.000000;-0.707107;,
  0.000000;-1.000000;-0.000000;;
  24;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;;
 }
 MeshTextureCoords {
  26;
  0.500000;0.996870;,
  0.869790;0.851050;,
  0.869790;0.698290;,
  0.500000;0.844110;,
  1.022960;0.499010;,
  1.022960;0.346260;,
  0.869790;0.146970;,
  0.869790;-0.005780;,
  0.500000;0.001150;,
  0.500000;-0.151600;,
  0.130210;0.146970;,
  0.130210;-0.005780;,
  -0.022960;0.499010;,
  -0.022960;0.346260;,
  0.130210;0.851050;,
  0.130210;0.698290;,
  0.500000;0.500000;,
  0.853553;0.853553;,
  0.500000;1.000000;,
  1.000000;0.500000;,
  0.853553;0.146447;,
  0.500000;0.000000;,
  0.146447;0.146447;,
  0.000000;0.500000;,
  0.146447;0.853553;,
  0.500000;0.346260;;
 }
}
