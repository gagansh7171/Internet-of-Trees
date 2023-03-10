import json
from channels.generic.websocket import WebsocketConsumer
from asgiref.sync import async_to_sync
from django.core.serializers import serialize
from .models import *
class TreeConsumer(WebsocketConsumer):
    def connect(self):
        self.group_name = "trees"
        async_to_sync(self.channel_layer.group_add)(
                    self.group_name,
                    self.channel_name
                )
        self.accept()
    
    def disconnect(self, close_code):
        async_to_sync(self.channel_layer.group_discard)(
            self.group_name,
            self.channel_name
        )
    
    def receive(self, text_data):
        async_to_sync(self.channel_layer.group_send)(
            self.group_name,
            {
                "type": "update_tree_list"
            },
        )
    
    def update_tree_list(self, event):
        # print(TreeModelSerial(TreeModel.objects.all(), many=True).data)
        self.send(
            text_data=json.dumps(
                {
                    "trees": TreeModelSerial(TreeModel.objects.all(), many=True).data ,
                }
            )
        )