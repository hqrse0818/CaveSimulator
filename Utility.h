#pragma once

#include	<iostream>
#include	<functional>
#include <SimpleMath.h>
#include <thread>

#include    <assimp\Importer.hpp>
#include    <assimp\scene.h>
#include    <assimp\postprocess.h>
#include    <assimp/cimport.h>

#include "Format.h"
#include "Debug.h"

#define Gravity     4.0
#define PIE			3.1415926535
#define Horizontal	180
#define OriginalGroundHeight  0.0
#define EulerGap	Horizontal / PIE	// ラジアンからオイラー角に直すとき
#define RadianGap	PIE / Horizontal	// オイラー角からラジアンに直すとき

#define Radian_To_Euler(Value) (Value * EulerGap)
#define Euler_To_Radian(Value) (Value * RadianGap)

#define GetMin(numA, numB)(numA < numB ? numA : numB)

#define GetMax(numA, numB)(numA > numB ? numA : numB)

#define BoxLineNum 12
#define RenderBoxVertexNum 24
#define BoxVertexNum 8

#define SAFE_RELEASE(p) {if(p!=nullptr){p->Release(); p = nullptr;}}
#define SAFE_NULL(p) {if(p != nullptr){delete p;p = nullptr;}}


// ファイルパスからファイル名だけを取得する
std::string GetFileName(std::string FilePath);
// ファイルパスからファイル名だけを取得する
std::wstring GetFileNameWide(std::string filepath);
// ファイルパスからファイル拡張子を取得する
std::string GetFileExt(std::string filepath);
// ファイルパスから親ディレクトリを取得する
std::wstring GetParentDirectoryWide(std::string filepath);

HRESULT CreateVertexBuffer(
	ID3D11Device* _device,
	unsigned int _stride,
	unsigned int _vertexnum,
	void* _data,
	ID3D11Buffer* _buffer);

HRESULT CreateVertexBufferWrite(
	ID3D11Device* _device,
	unsigned int _stride,
	unsigned int _vertexnum,
	void* _data,
	ID3D11Buffer** _buffer);

HRESULT CreateConstantBuffer(
	ID3D11Device* _device,
	unsigned int _bytewidth,
	unsigned int _bytestride,
	ID3D11Buffer** _buffer
	);

HRESULT CreateConstantBufferWrite(ID3D11Device* _device,
	unsigned int _bytewidth,
	unsigned int _bytestride,
	ID3D11Buffer** _buffer
);