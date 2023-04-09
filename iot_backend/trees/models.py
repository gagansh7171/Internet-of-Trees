from django.contrib.auth.models import User
from rest_framework import serializers, viewsets, status
from django.db import models
from django.dispatch import receiver
from django.db.models.signals import pre_save
from datetime import datetime, timezone 
from rest_framework.response import Response
from channels.layers import get_channel_layer
channel_layer = get_channel_layer()
from asgiref.sync import async_to_sync

class UserSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = User
        fields = ['url', 'username', 'email']

# ViewSets define the view behavior.
class UserViewSet(viewsets.ModelViewSet):
    queryset = User.objects.all()
    serializer_class = UserSerializer



class TreeModel(models.Model):
    NORMAL = '0'
    HIGH_TEMPERATURE = '1'
    VIBRATION_SENSE = '2'
    ACCELERATION_SENSE = '3'
    FALLEN = '4'
    STATEL_CHOICES = [
        (NORMAL, 'NORMAL'),
        (HIGH_TEMPERATURE, 'HIGH_TEMPERATURE'),
        (VIBRATION_SENSE, 'VIBRATION_SENSE'),
        (ACCELERATION_SENSE,'ACCELERATION_SENSE'),
        (FALLEN, 'FALLEN'),
    ]
    macid = models.CharField(max_length=30, unique = True)
    nick = models.CharField(max_length=30, blank=True, null=True)
    state = models.CharField(
        max_length=1,
        choices=STATEL_CHOICES,
        default=NORMAL,
        blank=True,
        null=True
    )
    last_update = models.DateTimeField(blank=True, null = True)

@receiver(pre_save, sender=TreeModel)
def set_default_treemodel(sender, instance, **kwargs):
    if not instance.nick:
        instance.nick = instance.macid

class TreeModelSerial(serializers.ModelSerializer):
    class Meta:
        model = TreeModel
        fields = '__all__'

class TreeViewSet(viewsets.ModelViewSet):
    queryset = TreeModel.objects.all()
    serializer_class = TreeModelSerial

    def perform_create(self, serializer):
        serializer.save(last_update = datetime.now(timezone.utc), state = '0')
        async_to_sync(channel_layer.group_send)(
            "trees",
            {
                "type": "update_tree_list"
            },
        )
    def perform_update(self, serializer):
        serializer.save()
        async_to_sync(channel_layer.group_send)(
            "trees",
            {
                "type": "update_tree_list"
            },
        )
    
    def update(self, request, *args, **kwargs):
        current_time = datetime.now(timezone.utc)
        
        temp = {}
        partial = kwargs.pop('partial', False)
        instance = self.get_object()
        
        temp['macid'] = instance.macid
        temp['nick'] = request.data.get('nick', instance.nick)
        if len(temp['nick']) == 0:
            temp['nick'] = instance.nick
        temp['state'] = request.data.get('state', instance.state)
        temp['last_update'] = current_time

        difference_time = current_time - instance.last_update

        if instance.state == '4':
            return Response(status=status.HTTP_500_INTERNAL_SERVER_ERROR) 
        
        if instance.state != '0' and (difference_time.total_seconds()/60) < 30:
            if len(temp['nick']) == 0:
                temp['nick'] = instance.nick
            if temp['state'] == '0':
                temp['state'] = instance.state
            temp['last_update'] = current_time
            serializer = self.get_serializer(instance, data=temp, partial=partial)
            serializer.is_valid(raise_exception=True)
            self.perform_update(serializer)
            async_to_sync(channel_layer.group_send)(
                "trees",
                {
                    "type": "update_tree_list"
                },
            )
            return Response( status=status.HTTP_206_PARTIAL_CONTENT) 
        
        serializer = self.get_serializer(instance, data=temp, partial=partial)
        serializer.is_valid(raise_exception=True)
        self.perform_update(serializer)

        if getattr(instance, '_prefetched_objects_cache', None):
            instance._prefetched_objects_cache = {}
        
        async_to_sync(channel_layer.group_send)(
            "trees",
            {
                "type": "update_tree_list"
            },
        )

        return Response(status=status.HTTP_200_OK)
