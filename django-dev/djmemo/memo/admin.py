from django.contrib import admin

from .models import *

@admin.register(Memo)
class MemoAdmin(admin.ModelAdmin):
    list_display = ["title", "get_tags", "pub_date"]

    def get_tags(self, obj):
        return ", ".join(tag.name for tag in obj.tags.all())
    get_tags.short_description = 'Tags'

admin.site.register(Tag)