import { Component, OnInit } from '@angular/core';
import { MAT_DIALOG_DATA, MatDialogRef } from '@angular/material/dialog';
import { Inject } from '@angular/core';

@Component({
  selector: 'app-project-desc-modal',
  templateUrl: './project-desc-modal.component.html',
  styleUrls: ['./project-desc-modal.component.css']
})
export class ProjectDescModalComponent implements OnInit {

  projname: string;
  projloc: string;
  id:number;
  sensors:number;

  constructor(public dialogRef: MatDialogRef<ProjectDescModalComponent>, @Inject(MAT_DIALOG_DATA) data) { 
  	this.projloc=data.location;
  	this.projname=data.name;
  	this.id=data.id;
  	this.sensors=data.sensors;
  }

  ngOnInit(): void {
  }

  closeModal() {
    this.dialogRef.close();

  }

}
