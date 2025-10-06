from django.db import models

class Tag(models.Model):
    name = models.CharField(max_length=50, unique=True)

    def __str__(self):
        return self.name

class Memo(models.Model):
    title = models.CharField(null=True, blank=True, max_length=200)
    text = models.CharField(max_length=10000)
    pub_date = models.DateTimeField("date created")
    tags = models.ManyToManyField(Tag, related_name="memos")

    def __str__(self):
        return self.text