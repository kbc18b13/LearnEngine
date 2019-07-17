#include "ConstBuffer.h"
#include "Graphic/GraphicEngine.h"

namespace LearnEngine {
ConstBuffer::ConstBuffer() : camera(), light(), object() {
}
ConstBuffer::~ConstBuffer() {
}

void ConstBuffer::Init() {
	D3D11_BUFFER_DESC bufDesc{};
	bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA res{};

	HRESULT result;

	bufDesc.ByteWidth = sizeof(camera);
	res.pSysMem = &camera;
	result = D3DDevice()->CreateBuffer(&bufDesc, &res, &cameraCBuf);
	if (FAILED(result))abort();

	bufDesc.ByteWidth = sizeof(light);
	res.pSysMem = &light;
	D3DDevice()->CreateBuffer(&bufDesc, &res, &lightCBuf);
	if (FAILED(result))abort();

	bufDesc.ByteWidth = sizeof(object);
	res.pSysMem = &object;
	D3DDevice()->CreateBuffer(&bufDesc, &res, &objectCBuf);
	if (FAILED(result))abort();

	D3DContext()->VSSetConstantBuffers(OBJECT_SLOT, 1, &objectCBuf.p);
	D3DContext()->VSSetConstantBuffers(CAMERA_SLOT, 1, &cameraCBuf.p);
	D3DContext()->PSSetConstantBuffers(LIGHT_SLOT, 1, &lightCBuf.p);
}

void ConstBuffer::UpdateCamera() {
	D3D11_MAPPED_SUBRESOURCE sub;
 	HRESULT res = D3DContext()->Map(cameraCBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	if (FAILED(res)) {
		abort();
	}
	*(static_cast<CameraCB*>(sub.pData)) = camera;
	D3DContext()->Unmap(cameraCBuf, 0);
}

void ConstBuffer::UpdateLight() {
	D3D11_MAPPED_SUBRESOURCE sub;
	HRESULT res = D3DContext()->Map(lightCBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	if (FAILED(res)) {
		abort();
	}
	*(static_cast<LightCB*>(sub.pData)) = light;
	D3DContext()->Unmap(lightCBuf, 0);
}

void ConstBuffer::UpdateObject() {
	D3D11_MAPPED_SUBRESOURCE sub;
	HRESULT res = D3DContext()->Map(objectCBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	if (FAILED(res)) {
		abort();
	}
	*(static_cast<ObjectCB*>(sub.pData)) = object;
	D3DContext()->Unmap(objectCBuf, 0);
}



}