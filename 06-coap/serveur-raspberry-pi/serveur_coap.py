import asyncio
import aiocoap
import aiocoap.resource as resource

class CapteurResource(resource.Resource):
    async def render_get(self, request):
        print("Requête GET reçue")
        return aiocoap.Message(code=aiocoap.CONTENT, payload=b"CoAP Server: OK")

    async def render_put(self, request):
        data = request.payload.decode()
        print("Données reçues :", data)
        return aiocoap.Message(code=aiocoap.CHANGED, payload=b"CoAP Server: Data Received")

    async def render_post(self, request):
        return await self.render_put(request)

async def main():
    root = resource.Site()
    root.add_resource(['capteur'], CapteurResource())
    await aiocoap.Context.create_server_context(root)
    print("Serveur CoAP démarré sur le port 5683")
    await asyncio.get_running_loop().create_future()

if __name__ == "__main__":
    asyncio.run(main())
