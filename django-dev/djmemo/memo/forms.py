from django import forms
from .models import Tag, Memo

class MemoForm(forms.ModelForm):
    class Meta:
        model = Memo
        fields = ['title', 'text', 'tags']
    
    text = forms.CharField(widget=forms.Textarea(attrs={'class': 'large-textarea'}))

class TagForm(forms.ModelForm):
    class Meta:
        model = Tag
        fields = ['name']
