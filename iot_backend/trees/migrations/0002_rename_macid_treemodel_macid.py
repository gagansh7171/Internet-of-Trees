# Generated by Django 4.1.7 on 2023-03-08 12:27

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('trees', '0001_initial'),
    ]

    operations = [
        migrations.RenameField(
            model_name='treemodel',
            old_name='macId',
            new_name='macid',
        ),
    ]
