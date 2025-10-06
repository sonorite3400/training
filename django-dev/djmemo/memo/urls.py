from django.contrib import admin
from django.urls import include, path
from .views import HomeView, MemoCreateView, TagListView, TagCreateView,  MemoDetailView, MemoUpdateView, TagView

urlpatterns = [
    path('', HomeView.as_view(), name='home'),
    path('add_memo/', MemoCreateView.as_view(), name='add_memo'),
    path('tags/', TagListView.as_view(), name='tag_list'),
    path('tags/add', TagCreateView.as_view(), name='tag_create'),
    path('<int:pk>/', MemoDetailView.as_view(), name='memo_detail'),
    path('<int:pk>/edit/', MemoUpdateView.as_view(), name='memo_edit'),
    path('tag/<str:tag_name>/', TagView.as_view(), name='tag_detail')
]