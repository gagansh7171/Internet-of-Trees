from channels.routing import ProtocolTypeRouter, URLRouter
from channels.auth import AuthMiddlewareStack
from django.urls import re_path
from trees.consumers import TreeConsumer
application = ProtocolTypeRouter({
    'websocket': AuthMiddlewareStack(
        URLRouter(
            [ re_path(r'ws/tree/$', TreeConsumer.as_asgi()) ]
        )
    ),
})