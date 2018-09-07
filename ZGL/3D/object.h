#include "../inc.h"
#include "./piece.h"
#include "./vec4.h"

#ifndef ZGL_OBJECT
#define ZGL_OBJECT

_ZGL_BEGIN

class object {
	typedef z_size_t			size;
	typedef z_item_t			item;
	typedef object				self;
	typedef square < 4, item >	squ4;

public :
	piece* pieces;
	size count;

	~object() {
		if (pieces) {
			delete[] pieces;
		}
	}

	self() {
		this->count = 0;
		this->pieces = nullptr;
	}

	self( size count ) {
		this->count = count;
		this->pieces = new piece[this->count];
	}

	self(const piece* src, size count)
		: self( count ){
		for (size i = 0; i < count; i++) {
			this->pieces[i] = src[i];
		}
	}

	self(const self& src)
		: self(src.pieces, src.count) {
	}

	/**
	 * @positions		顶点集
	 * @positionNum		顶点数
	 * @indices			索引集
	 * @indiceNum		索引数
	 **/
	self(const item* positions, const size positionNum, const size* indices, const size indiceNum)
		: self(indiceNum / 3) {
		vec4* vecs = new vec4[positionNum / 3];
		for (size i = 0; i < positionNum; i += 3) {
			vecs[i / 3] = { positions[i], positions[i + 1], positions[i + 2], item(1) };
		}

		for (size i = 0; i < indiceNum; i += 3) {
			this->pieces[i / 3] = { vecs[indices[i]], vecs[indices[i + 1]], vecs[indices[i + 2]] };
		}

		delete[] vecs;
	}

#ifdef ZGL_ENABLE_RVALUE
	self(self&& src) {
		if (this != &src) {
			delete[] this->pieces;
			this->pieces = src.pieces;
			src.pieces = nullptr;
			this->count = src.count;
		}
	}
#endif

	self& operator = (const self& src) {
		this->count = src.count;
		this->pieces = new piece[this->count];
		for (size i = 0; i < this->count; i++) {
			this->pieces[i] = src.pieces[i];
		}

		return *this;
	}

	piece& operator [] (size opt) {
		if (opt >= count) {
			throw "out of range.";
		}
		return pieces[opt];
	}

	const self& translate(const vec4& opt) {
		squ4 transSqu = vec4::Translate(opt);
		this->transform(transSqu);

		return *this;
	}

	const self& rotate(const vec4& position, const vec4& normal, item radian) {
		squ4 rotateSqu = vec4::Rotate(position, normal, radian);
		this->transform(rotateSqu);
		
		return *this;
	}

	const self& rotate(item yaw, item pitch, item roll) {
		squ4 rotateSqu = vec4::Rotate(yaw, pitch, roll);
		this->transform(rotateSqu);

		return *this;
	}

	const self& scale(item opt) {
		squ4 scaleSqu = vec4::Scale(opt);
		this->transform(scaleSqu);

		return *this;
	}

	self& transform(const squ4& squ) {
		for (size i = 0; i < this->count; i++) {
			this->pieces[i].transform(squ);
		}

		return *this;
	}
};

_ZGL_END

#endif