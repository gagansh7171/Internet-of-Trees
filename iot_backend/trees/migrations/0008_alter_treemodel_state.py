# Generated by Django 4.1.7 on 2023-03-08 14:50

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('trees', '0007_alter_treemodel_last_update'),
    ]

    operations = [
        migrations.AlterField(
            model_name='treemodel',
            name='state',
            field=models.CharField(blank=True, choices=[('0', 'NORMAL'), ('1', 'HIGH_TEMPERATURE'), ('2', 'VIBRATION_SENSE'), ('3', 'ACCELERATION_SENSE'), ('4', 'FALLEN')], default='0', max_length=1),
        ),
    ]
